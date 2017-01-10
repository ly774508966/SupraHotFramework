using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SupraHot.CLI;

namespace SupraHot
{
    class FileSystemUtil
    {
        public static String IsInContent(String filePath) 
        {

            FileSystemCLI fileSystem = FileSystemCLI.GetIntance();
            String fileSystemRootPath = fileSystem.GetRootPath();

            filePath = filePath.Replace("\\", "/");

            if (filePath.Contains("Content/"))
            {
                var splittedString = filePath.Split(new[] { "Content/" }, StringSplitOptions.None);

                if (splittedString.Length > 1)
                {
                    var pathToFileInRootSys = splittedString[1];

                    if (fileSystem.FileExists(pathToFileInRootSys))
                    {
                        return pathToFileInRootSys;
                    } 
                }

                return null;
            }

            return null;

        }
    }
}
