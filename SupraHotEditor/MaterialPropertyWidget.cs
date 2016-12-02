using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SupraHot.CLI;

namespace SupraHotEditor
{
    class MaterialPropertyWidget : GroupBox
    {
        private MaterialPropertyCommonInterface MaterialPropertyCommonInterface = null;
        private String Type = "";
        private String Name = "";
        private FlowLayoutPanel groupBoxFlowLayout;

        public MaterialPropertyWidget(MaterialPropertyCommonInterface materialPropertyCommonInterface) 
        {
            this.MaterialPropertyCommonInterface = materialPropertyCommonInterface;
            this.Type = materialPropertyCommonInterface.GetType();
            this.Text = materialPropertyCommonInterface.GetName() + "( " + this.Type + " )";

            groupBoxFlowLayout = new FlowLayoutPanel();
            groupBoxFlowLayout.Dock = DockStyle.Fill;
            this.Controls.Add(groupBoxFlowLayout);

            if(Type == "Boolean") 
            {
                RadioButton trueButton = new RadioButton();
                trueButton.Text = "True";
                trueButton.Margin = new Padding(0, 0, 0, 0);

                RadioButton falseButton = new RadioButton();
                falseButton.Text = "False";
                falseButton.Margin = new Padding(0, 0, 0, 0);

                groupBoxFlowLayout.Controls.Add(trueButton);
                groupBoxFlowLayout.Controls.Add(falseButton);

                trueButton.CheckedChanged += new EventHandler(
                    delegate (object sender, EventArgs e) {

                        if (trueButton.Checked)
                        {
                            ((BooleanMaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(true);
                        }
                        else if (falseButton.Checked)
                        {
                            ((BooleanMaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(false);
                        }
                    }
                );
            } 
            else if(Type == "Vec3") {
                Console.WriteLine("Vec3");
            }
        }
    }
}
