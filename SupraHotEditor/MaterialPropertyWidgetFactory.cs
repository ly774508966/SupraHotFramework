using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SupraHot.CLI;

namespace SupraHotEditor
{
    class MaterialPropertyWidgetFactory
    {
        static MaterialPropertyWidget CreateGroupBox(MaterialPropertyCommonInterface materialPropertyCommonInterface) 
        {
            return new MaterialPropertyWidget(materialPropertyCommonInterface);
        }
    }
}
