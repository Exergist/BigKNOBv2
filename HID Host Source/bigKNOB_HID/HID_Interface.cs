using System;
using HidLibrary;

namespace bigKNOB_HID
{
    public class HID_Interface
    {
        #region Fields & Objects

        private static HidDevice kbDevice;
        private static int vendorID;
        private static int productID;
        private static int usagePage;
        private static int usageID;

        #endregion

        #region Methods

        // Method for connecting with a HidDevice 
        public static void Connect(int vID, int pID, int uPage, int uID)
        {
            // Transfer passed-in target HidDevice information
            vendorID = vID;
            productID = pID;
            usagePage = uPage;
            usageID = uID;

            try // Attempt the following code...
            {
                var devices = HidDevices.Enumerate(vendorID, productID, usagePage); // Capture all HidDevices

                foreach (HidDevice dev in devices) // Loop through each HidDevice
                {
                    if (dev.Capabilities.Usage == usageID) // Check if current HidDevice matches target device's usageID
                    {
                        Console.WriteLine("Target HidDevice found!"); // Output info to event log (debug)
                        kbDevice = dev; // Store the found HidDevice
                        break; // Break out of parent 'foreach' loop
                    }
                }
                if (kbDevice == null) // Check if target HidDevice was not found
                    Console.WriteLine("Could not find target HidDevice"); // Output info to event log
            }
            catch (Exception ex) // Handle exceptions encountered in above code
            {
                Console.WriteLine("Could not connect to target HidDevice." + Environment.NewLine + ex.Message); // Output info to event log
            }
        }

        // Method for communicating with a HidDevice
        public static void SendReceive(int action, int context, bool retry = false)
        {
            try // Attempt the following code...
            {
                // Check status of kbDevice connection and (if applicable) attempt to reconnect
                if (kbDevice?.IsConnected != true) // Check if kbDevice is NOT connected to host computer
                {
                    string connectionFailedMessage = "Could not connect to target HidDevice"; // Store output message
                    if (retry == true) // Check if connection with kbDevice should be reattempted
                    {
                        Connect(vendorID, productID, usagePage, usageID); // Call method to connect with target HidDevice
                        if (kbDevice?.IsConnected != true) // Check if kbDevice is (still) NOT connected to host computer
                        {
                            Console.WriteLine(connectionFailedMessage); // Output info to event log
                            return; // Return from this method
                        }
                    }
                    else
                    {
                        Console.WriteLine(connectionFailedMessage); // Output info to event log
                        return; // Return from this method
                    }
                }

                // Initialize byte array for sending info to QMK
                byte[] OutData = new byte[kbDevice.Capabilities.OutputReportByteLength - 1];

                // Enter info for sending to QMK
                OutData[0] = 0; // 'Report ID' not received by QMK, so set to zero
                OutData[1] = (byte)action; // Action for QMK to execute
                OutData[2] = (byte)context; // Context for desired QMK action

                // Send OutData to QMK
                Console.WriteLine("Sending data to QMK"); // Output info to event log (debug)
                kbDevice.Write(OutData); // Send OutData to QMK

                /* // Read data received from QMK
                HidDeviceData InData = kbDevice.Read(1000); // Read data from QMK (with timeout of 1000 ms?) (debug)
                string Text = System.Text.ASCIIEncoding.ASCII.GetString(InData.Data); // Convert received (byte) data to string (debug)
                Console.WriteLine(Text); // Output info to event log (debug) */
            }
            catch (Exception ex) // Handle exceptions encountered in above code
            {
                Console.WriteLine("Failed to exchange data with target HidDevice." + Environment.NewLine + ex.InnerException); // Output info to event log
            }
        }

        // Method for closing HidDevice interface
        public static void Close()
        {
            if (kbDevice != null) // Check if kbDevice is still 'active'
            {
                Console.WriteLine("Closing HidDevice interface"); // Output info to event log (debug)
                kbDevice.CloseDevice(); // Close connection with kbDevice
                kbDevice.Dispose(); // Dispose of kbDevice instance
                kbDevice = null; // Set kbDevice instance as null
            }
        }

        #endregion
    }
}
