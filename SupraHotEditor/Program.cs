using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using SupraHot.Graphics;

namespace SupraHotEditor
{
    static class Program
    {
        public static WindowEditor SupraHotWindow;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            var form = new Form1();

            unsafe
            {
                void* hwndPointer = form.Handle.ToPointer();

                SupraHotWindow = new WindowEditor();
                SupraHotWindow.SetHWND(hwndPointer);
                SupraHotWindow.Init(0, 0);
                SupraHotWindow.SetClearColor(1, 0, 0, 1);
            }

            Application.Run(form);
        }
    }
}
