using HidLibrary;
using System;
using System.Linq;

namespace Ex.HidInterface
{
    public class HostInterface
    {
        #region Fields & Objects

        private HidDevice kbDevice;
        private int _vendorID;
        private int _productID;
        private int _usagePage;
        private int _usageID;
        private string hidDeviceName;

        #endregion

        #region Enumeration

        // Enumeration for actions to execute by HidDevice
        public enum HidAction
        {
            ChangeLayer = 1
        }

        #endregion

        #region Properties

        // Property indicating if HostInterface (computer) is connected to HidDevice
        public bool IsConnected
        {
            get { return kbDevice.IsConnected; }
        }

        // Property indicating if HostInterface (computer) is listening for messages from HidDevice
        private bool _isListening = false;
        public bool IsListening
        {
            get { return _isListening; }
            set
            {
                if (_isListening != value)
                {
                    _isListening = value;
                    if (_isListening == true)
                    {
                        kbDevice.MonitorDeviceEvents = true; // Enable HidDevice event monitoring
                        kbDevice.ReadReport(OnReport); // Subscribe to OnReport (as callback) for next received message from HidDevice
                    }
                    else
                        kbDevice.MonitorDeviceEvents = false; // Disable HidDevice event monitoring
                }
            }
        }

        #endregion

        #region Constructor

        // Method for creating HostInterface instance
        public HostInterface(string deviceName, string vendorID, string productID, string usagePage = null, string usageID = null)
        {
            // Convert passed-in hexadecimal strings (for target HidDevice) to integers
            _vendorID = ConvertHexStringToInt(vendorID);
            _productID = ConvertHexStringToInt(productID);
            if (usagePage != null)
                _usagePage = ConvertHexStringToInt(usagePage);
            else
                _usagePage = -1;
            if (usageID != null)
                _usageID = ConvertHexStringToInt(usageID);
            else
                _usageID = -1;

            // Transfer passed-in target HidDevice name
            hidDeviceName = deviceName;
        }

        #endregion

        #region Interface Methods

        // Method for connecting with a HidDevice 
        public void Connect(bool hidDeviceListeningEnabled = true)
        {
            try // Attempt the following code...
            {
                if (_usagePage == -1 || _usageID == -1) // Check if usagePage OR usageID were not provided when HostInterface was instantiated
                    kbDevice = HidDevices.Enumerate(_vendorID, _productID).FirstOrDefault(); // Find first HidDevice that matches _vendorID and _productID
                else
                {
                    var devices = HidDevices.Enumerate(_vendorID, _productID, _usagePage); // Capture all HidDevices
                    ///throw new Exception("test"); // (debug)
                    foreach (HidDevice dev in devices) // Loop through each HidDevice
                    {
                        if (dev.Capabilities.Usage == _usageID) // Check if current HidDevice matches target device's usageID
                        {
                            kbDevice = dev; // Store the found HidDevice
                            break; // Break out of parent 'foreach' loop
                        }
                    }
                }
                if (kbDevice != null) // Check if target HidDevice not found
                {
                    Console.WriteLine(hidDeviceName + " found!"); // Output info to event log (debug)
                    kbDevice.OpenDevice(); // Open connection between HostInterface (computer) and HidDevice
                    IsListening = hidDeviceListeningEnabled; // Transfer passed-in listening state (and activate listening for HidDevice data messages if applicable)
                    kbDevice.Inserted += DeviceAttachedHandler; // Subscribe to HidDevice attachment events
                    kbDevice.Removed += DeviceRemovedHandler; // Subscribe to HidDevice removal events
                }
                else
                    Console.WriteLine("Could not find " + hidDeviceName); // Output info to event log
            }
            catch (Exception ex) // Handle exceptions encountered in above code
            {
                Console.WriteLine("Error connecting to " + hidDeviceName + "." + ex.Message); // Output info to event log
            }
        }

        // Method for checking HidDevice connection with HostInterface (computer) and reconnecting if applicable
        private bool CheckConnection(bool retry = false)
        {
            bool result = false; // Initialize variable for storing processing result
            try // Attempt the following code...
            {
                if (kbDevice?.IsConnected != true) // Check if kbDevice is NOT connected to HostInterface computer
                {
                    string connectionFailedMessage = hidDeviceName + " is not connected"; // Store output message
                    if (retry == true) // Check if connection with kbDevice should be reattempted
                    {
                        Connect(IsListening); // Call method to connect with target HidDevice
                        if (kbDevice?.IsConnected != true) // Check if kbDevice is (still) NOT connected to HostInterface (computer)
                            Console.WriteLine(connectionFailedMessage); // Output info to event log
                        else
                            result = true; // Update result
                    }
                    else
                        Console.WriteLine(connectionFailedMessage); // Output info to event log
                }
                else
                    result = true; // Update result
            }
            catch (Exception ex) // Handle exceptions encountered in above code
            {
                Console.WriteLine("Error checking connection with " + hidDeviceName + "." + ex.Message); // Output info to event log
            }
            // if (result == true) // Check if result is TRUE (debug)
                // Console.WriteLine(hidDeviceName + " is connected"); // Output info to event log (debug)
            return result;
        }

        // Method for sending data to a HidDevice
        public bool Send(int action, int context, bool retry = false)
        {
            ///throw new Exception("test"); // (debug)
            bool result = false; // Initialize variable for storing processing result
            try // Attempt the following code...
            {
                if (CheckConnection(retry) == false) // Check if HostInterface (computer) is NOT connected with target HidDevice 
                    return result; // Return result from this method

                // Initialize byte array for sending info to HidDevice
                byte[] OutData = new byte[kbDevice.Capabilities.OutputReportByteLength - 1];

                // Enter info for sending to HidDevice
                // This is configured to communicate with QMK (Raw HID)
                OutData[0] = 0; // 'Report ID' not received by QMK, so set to zero
                OutData[1] = (byte)action; // Action for HidDevice to execute
                OutData[2] = (byte)context; // Context for desired HidAction

                // Send OutData to HidDevice
                Console.WriteLine("Sending data to " + hidDeviceName); // Output info to event log (debug)
                if (kbDevice.Write(OutData) == false) // Send OutData to HidDevice and check if process was NOT successful
                    Console.WriteLine("Could not send data to " + hidDeviceName); // Output info to event log
                else
                    result = true; // Update result
            }
            catch (Exception ex) // Handle exceptions encountered in above code
            {
                Console.WriteLine("Error sending data to " + hidDeviceName + "." + ex.Message); // Output info to event log
            }
            return result; // Return result from this method
        }

        // Method for (manually) receiving data from a HidDevice
        public bool Receive(bool retry = false)
        {
            ///throw new Exception("test"); // (debug)
            bool result = false; // Initialize variable for storing processing result
            try // Attempt the following code...
            {
                if (CheckConnection(retry) == false) // Check if HostInterface (computer) is NOT connected with target HidDevice 
                    return result; // Return result from this method

                // Read data received from HidDevice
                HidDeviceData InData = kbDevice.Read(1000); // Read data from HidDevice (with timeout of 1000 ms?)
                if (InData.Status != HidDeviceData.ReadStatus.Success) // Check if reading data from HidDevice was NOT successful
                    Console.WriteLine("Could not read data from " + hidDeviceName); // Output info to event log
                else
                {
                    int[] data = Array.ConvertAll(InData.Data, c => (int)c); // Convert received byte data to integer array
                    Console.WriteLine(Convert.ToChar(data[1])); // Output info to event log (debug)
                    result = true; // Update result
                }
            }
            catch (Exception ex) // Handle exceptions encountered in above code
            {
                Console.WriteLine("Error receiving data from " + hidDeviceName + "." + ex.Message); // Output info to event log
            }
            return result; // Return result from this method
        }

        // Method for closing HidDevice interface
        public void Close()
        {
            if (kbDevice != null) // Check if kbDevice is still 'active'
            {
                Console.WriteLine("Closing connection with " + hidDeviceName); // Output info to event log (debug)
                IsListening = false; // Disable HostInterface (computer) listening for HidDevice messages
                kbDevice.Inserted -= DeviceAttachedHandler; // Unsubscribe from HidDevice attachment events
                kbDevice.Removed -= DeviceRemovedHandler; // Unsubscribe from HidDevice removal events
                kbDevice.CloseDevice(); // Close connection with kbDevice
                kbDevice.Dispose(); // Dispose of kbDevice instance
                kbDevice = null; // Set kbDevice instance as null
            }
        }

        #endregion

        #region Event Methods

        // Method run when HidDevice is attached (with HostInterface having previously identified the HidDevice)
        private void DeviceAttachedHandler()
        {
            Console.WriteLine(hidDeviceName + " attached"); // Output info to event log (debug)
        }

        // Method run when HidDevice is removed (with HostInterface having previously identified the HidDevice)
        private void DeviceRemovedHandler()
        {
            Console.WriteLine(hidDeviceName + " removed"); // Output info to event log (debug)
        }

        // Method run when HidDevice sends data to the (connected and listening) HostInterface (computer)
        private void OnReport(HidReport report)
        {
            if (IsConnected == false || IsListening == false) // Check if HostInterface (computer) is NOT connected to HidDevice OR is NOT listening for HidDevice messages 
                return; // Return from this method

            if (report.Data.Length >= 4) // Check if length of received data is greater than or equal to 4 elements
            {
                int[] data = Array.ConvertAll(report.Data, c => (int)c); // Convert received byte data to integer array
                Console.WriteLine(Convert.ToChar(data[0])); // Output info to event log (debug)
            }

            kbDevice.ReadReport(OnReport); // Subscribe to OnReport (as callback) for next received message from HidDevice
        }

        #endregion

        #region Processing Methods

        // Method for converting hexadecimal string into integer
        private static int ConvertHexStringToInt(string hexString)
        {
            int intValue;
            if (hexString.StartsWith("0x") == true) // Handle case where hex string is prefixed with "0x" (e.g., 0x61)
                intValue = Convert.ToInt32(hexString, 16);
            else
                intValue = int.Parse(hexString, System.Globalization.NumberStyles.HexNumber);
            return intValue;
        }

        #endregion
    }
}

#region Acknowledgements

// Mike O'Brien and Austin Mullins (and other contributors) for HidLibrary (https://github.com/mikeobrien/HidLibrary)
// Dasky and fauxpark from the QMK Discord for sharing their code and offering advice during development

#endregion

#region References

// Hex string to int conversion ==> https://theburningmonk.com/2010/02/converting-hex-to-int-in-csharp/
// ASCII to Hex Conversion ==> https://www.rapidtables.com/convert/number/ascii-hex-bin-dec-converter.html
// 'MagtekCardReader' example from HidLibrary v3.3.40 ==> https://github.com/mikeobrien/HidLibrary/releases

#endregion