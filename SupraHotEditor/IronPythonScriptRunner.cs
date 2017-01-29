using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Scripting;
using Microsoft.Scripting.Hosting;
using IronPython.Hosting;
using SupraHot.CLI;

namespace SupraHotEditor
{
    public class IronPythonScriptRunner
    {
        private static IronPythonScriptRunner Instance;
        private ScriptRuntime ScriptRuntime;
        private ScriptEngine ScriptEngine;
        private ScriptScope ScriptScope;
        
        private IronPythonScriptRunner() 
        {
            ScriptEngine = Python.CreateEngine();
            ScriptScope = ScriptEngine.CreateScope();
            ScriptRuntime = ScriptEngine.Runtime;
            ScriptRuntime.LoadAssembly(typeof(String).Assembly);
            ScriptRuntime.LoadAssembly(typeof(Uri).Assembly);

            var paths = ScriptEngine.GetSearchPaths();
            paths.Add(FileSystemCLI.GetIntance().GetRootPath() + "PythonLibraries");
            ScriptEngine.SetSearchPaths(paths);
        }

        public void RunScript(String pathToFile) 
        {
            String script = System.IO.File.ReadAllText(pathToFile);
            var source = ScriptEngine.CreateScriptSourceFromString(script, SourceCodeKind.Statements);

            try
            {
                source.Execute(ScriptScope);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Oops! There was an exception while running the script:\r\n" + ex.Message + "\r\n\r\n" + ex.StackTrace);
            }


        }

        public static IronPythonScriptRunner GetInstance() 
        {
            if(Instance == null) 
            {
                Instance = new IronPythonScriptRunner();
            }

            return Instance;
        }
    }
}
