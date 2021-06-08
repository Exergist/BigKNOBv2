using System;

namespace bigKNOB_HID
{
    class bigKNOB_Interface
    {
        #region Enumeration

        // Enumeration for actions to execute in QMK (BigKNOB)
        public enum BigKnobAction
        {
            ChangeLayer = 1
        }

        #endregion

        static void Main(string[] args)
        {
            // Target HidDevice (BigKNOB) information
            int vendorID = 0xCEEB;
            int productID = 0x0007;
            int usagePage = 0xFF60;
            int usageID = 0x61;

            BigKnobAction action = BigKnobAction.ChangeLayer; // Action for QMK to execute
            int context = 0; // Context for desired BigKnobAction

            HID_Interface.Connect(vendorID, productID, usagePage, usageID); // Connect with target HidDevice
            HID_Interface.SendReceive((int)action, context); // Send data to (and potentially receive data from) target HidDevice
            HID_Interface.Close(); // Close HidDevice interface

            Console.WriteLine("press any key to continue"); // Output info to event log (debug)
            Console.ReadLine(); // debug
        }
    }
}

#region Acknowledgements

// Mike O'Brien and Austin Mullins (and other contributors) for HidLibrary (https://github.com/mikeobrien/HidLibrary)
// Dasky and fauxpark from the QMK Discord for sharing their code and offering advice during development

#endregion

#region Supplemental References

// ASCII to Hex Conversion ==> https://www.rapidtables.com/convert/number/ascii-hex-bin-dec-converter.html

#endregion