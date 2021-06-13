// VAHidInterface v0.1.0
// Plugin providing two-way communication between VoiceAttack and connected HID hardware
// Compatible with 32-bit and 64-bit VoiceAttack version 1.8.8 or later
// Uses VoiceAttack plugin interface version 4


// TODO
// change required version to 1.8.8 when ready
// compare and update Ex.HidInterface Code vs the HostInterface code here
// possibly add better error logging
// introduce mechanism for user to input HID hardware info (instead of it being hardcoded below)


using System;

namespace VA.HidInterface
{
    public static class VoiceAttackPlugin
    {
        #region Fields & Objects

        private static bool vaVersionCompatible = false;
        private static dynamic VA;
        private static HostInterface hostInterface;
        private static readonly Version requiredVaVersion = new Version(1, 8, 7); /// update this for new 1.8.8 version!

        #endregion

        #region Enumeration

        private enum HidInterfaceAction
        {
            Connect,
            Disconnect,
            Check,
            Send
        }

        #endregion

        #region VoiceAttack Plugin Methods

        // Unique identifier for the VoiceAttack plugin
        public static Guid VA_Id()
        {
            return new Guid("{56622E9D-8D40-4C13-B0F0-47611B543C26}");
        }

        // What VoiceAttack will display when referring to the plugin
        public static string VA_DisplayName()
        {
            return "VAHidInterface - a0.1.0";
        }

        // Display extra information about the VoiceAttack plugin
        public static string VA_DisplayInfo()
        {
            return "VAHidInterface\r\n\r\nTwo-way communication between VoiceAttack and HID hardware \r\n\r\n2021 Exergist";
        }

        // Initialization method run when VoiceAttack starts
        public static void VA_Init1(dynamic vaProxy)
        {
            VA = vaProxy; // "Store" VA proxy for access outside of VA_Init1 and VA_Invoke1
            if (vaProxy.VAVersion >= requiredVaVersion)
            {
                vaVersionCompatible = true;

                // Target HID hardware information (with identifying info as hexadecimal strings)
                string deviceName = "bigKNOBv2";
                string vendorID = "0xCEEB";
                string productID = "0x0007";
                string usagePage = "0xFF60";
                string usageID = "0x61";

                // Create new HostInterface instance and pass it target HID hardware's information
                // It is strongly recommended that all the below information be provided (your mileage may vary)
                hostInterface = new HostInterface(deviceName, vendorID, productID, usagePage, usageID);

                // Connect with target HID hardware and engage automatic 'listening' for HID hardware data messages
                hostInterface.Connect(true);
            }
            else
                OutputToLog(VA_DisplayName() + " requires VoiceAttack v" + requiredVaVersion.ToString() + " or later, but v" + vaProxy.VAVersion.ToString() + " is currently installed", "red");
        }

        // Method run when VoiceAttack closes
        public static void VA_Exit1(dynamic vaProxy)
        {
            // Close interface with HID hardware
            hostInterface.Close();
        }

        // Method run when a "stop all commands" VoiceAttack action is performed
        public static void VA_StopCommand()
        {
        }

        // Method called when an "execute external plugin function" VoiceAttack action (targeting VAHidInterface) is performed
        public static void VA_Invoke1(dynamic vaProxy)
        {
            #region Pre-processing

            if (vaVersionCompatible == false)
            {
                OutputToLog(VA_DisplayName() + " requires VoiceAttack v" + requiredVaVersion.ToString() + " or later, but v" + vaProxy.VAVersion.ToString() + " is currently installed", "red");
                return;
            }
            string inputContext = vaProxy.Context;
            string[] context = Array.ConvertAll(inputContext.Split(':'), p => p.Trim());
            if (Enum.TryParse(context[0], true, out HidInterfaceAction hidInterfaceAction) == false)
            {
                OutputToLog("'" + context[0].ToLower() + "' is invalid VAHidInterface action", "red");
                return;
            }
            ///OutputToLog(action.ToString().ToLower(), "purple"); // debug

            #endregion

            #region Process requested action

            bool invalidContext = false;
            switch (hidInterfaceAction)
            {
                case HidInterfaceAction.Connect: // Connect VoiceAttack with target HID hardware. Context ==> connect
                    if (context.Length == 1)
                    {
                        if (hostInterface.IsActive == false)
                            hostInterface.Connect(); // Call method to connect VoiceAttack with target HID hardware
                        else if (hostInterface.IsConnected == false)
                            OutputToLog("Interface between VoiceAttack and " + hostInterface.DeviceName + " is already active", "yellow");
                        else
                            OutputToLog("VoiceAttack is already connected with " + hostInterface.DeviceName, "yellow");
                    }
                    else
                        invalidContext = true;
                    break;
                case HidInterfaceAction.Disconnect: // Disconnect VoiceAttack from target HID hardware. Context ==> disconnect
                    if (context.Length == 1)
                    {
                        if (hostInterface.IsActive == true)
                            hostInterface.Close(); // Call method to disconnect VoiceAttack from target HID hardware
                        else
                            OutputToLog("Interface between VoiceAttack and " + hostInterface.DeviceName + " is not currently active", "yellow");
                    }
                    else
                        invalidContext = true;
                    break;
                case HidInterfaceAction.Check: // Check status of connection between VoiceAttack and target HID hardware. Context ==> status
                    if (context.Length == 1)
                    {
                        string message = "VoiceAttack is " + (hostInterface.IsConnected == true ? "connected" : "not connected") + " with " + hostInterface.DeviceName;
                        if (hostInterface.IsActive == true)
                            message += ", and interface is active";
                        else
                            message += ", and interface is inactive";
                        OutputToLog(message, "blue");
                    }
                    else
                        invalidContext = true;
                    break;
                case HidInterfaceAction.Send: // Send data from VoiceAttack to target HID hardware. Context ==> send : HidAction : context
                    if (context.Length == 2 || context.Length == 3)
                    {
                        if (Enum.TryParse(context[1], true, out HostInterface.HidAction hidAction) == true)
                        {
                            if (context.Length == 2)
                                hostInterface.Send((int)hidAction);
                            else
                            {
                                if (Int32.TryParse(context[2], out int hidActionContext) == true)
                                {
                                    if (hostInterface.Send((int)hidAction, hidActionContext) == false)
                                        OutputToLog("Could not perform VAHidInterface '" + hidInterfaceAction + "' action", "red");
                                }
                                else
                                    OutputToLog("'" + context[2] + "' is invalid context for HidAction '" + hidAction + "'", "red");
                            }
                        }
                        else
                            OutputToLog("'" + context[1] + "' is invalid HidAction for VAHidInterface '" + hidInterfaceAction + "' action", "red");
                    }
                    else
                        invalidContext = true;
                    break;
            }
            if (invalidContext == true)
                OutputToLog("'" + inputContext + "' contains invalid context for VAHidInterface '" + hidInterfaceAction.ToString().ToLower() + "' action", "red");

            #endregion
        }

        #endregion

        #region Processing Methods

        // Method for outputting info to event log
        public static void OutputToLog(string message, string color = "blank")
        {
            VA.WriteToLog(message, color);
        }

        #endregion

    }
}
