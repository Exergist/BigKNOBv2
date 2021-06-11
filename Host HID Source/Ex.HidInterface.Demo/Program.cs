using System;

namespace Ex.HidInterface.Demo
{
    class Program
    {
        static void Main(string[] args)
        {
            // Target HidDevice information (with identifying info as hexadecimal strings)
            string deviceName = "bigKNOBv2";
            string vendorID = "0xCEEB";
            string productID = "0x0007";
            string usagePage = "0xFF60";
            string usageID = "0x61";

            // Requested action and its context for HidDevice
            HostInterface.HidAction action = HostInterface.HidAction.ChangeLayer; // Action to execute on HidDevice
            int context = 1; // Context for desired HidAction

            // Create new HostInterface instance and pass it target HidDevice's information
            // It is strongly recommended that all the below information be provided (your mileage may vary)
            HostInterface hostInterface = new HostInterface(deviceName, vendorID, productID, usagePage, usageID);

            // Connect with target HidDevice and engage automatic 'listening' for HidDevice data messages
            hostInterface.Connect(true); 

            Console.WriteLine("press any key to continue"); // Output info to event log (debug)
            Console.ReadLine();

            // Send data to target HidDevice
            hostInterface.Send((int)action, context);

            /* // (manually) Receive data from target HidDevice
            // Recommended to use automatic 'listening' instead of this (engaged via hostInterface.Connect seen above)
            hostInterface.Receive(); */

            Console.WriteLine("press any key to continue"); // Output info to event log (debug)
            Console.ReadLine();

            // Close HidDevice interface
            hostInterface.Close();

            // Debug for the event log
            Console.WriteLine("press any key to continue"); // Output info to event log (debug)
            Console.ReadLine(); // debug
        }
    }
}
