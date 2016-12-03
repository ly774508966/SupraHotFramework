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
            this.Text = materialPropertyCommonInterface.GetName() + " (" + this.Type + ")";

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
                falseButton.Checked = true;

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

                        Form1.UpdateView();
                    }
                );
            }
            else if (Type == "Float")
            {

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;

                groupBoxFlowLayout.Controls.Add(xLabel);
                groupBoxFlowLayout.Controls.Add(xSpinner);

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        // TODO: Impl. float materialproperty
                        Form1.UpdateView();
                    }
                );
            }
            else if (Type == "Vec2")
            {

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;

                groupBoxFlowLayout.Controls.Add(xLabel);
                groupBoxFlowLayout.Controls.Add(xSpinner);

                groupBoxFlowLayout.Controls.Add(yLabel);
                groupBoxFlowLayout.Controls.Add(ySpinner);

                this.Height = 200;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                       //Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                       //((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        //Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        //((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

            }
            else if(Type == "Vec3") 
            {

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 6;
                zSpinner.Increment = 0.1M;

                groupBoxFlowLayout.Controls.Add(xLabel);
                groupBoxFlowLayout.Controls.Add(xSpinner);

                groupBoxFlowLayout.Controls.Add(yLabel);
                groupBoxFlowLayout.Controls.Add(ySpinner);

                groupBoxFlowLayout.Controls.Add(zLabel);
                groupBoxFlowLayout.Controls.Add(zSpinner);

                this.Height = 200;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        ((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        ((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        ((Vec3MaterialPropertyCLI)this.MaterialPropertyCommonInterface).SetValue(result);
                        Form1.UpdateView();
                    }
                );
            }
            else if (Type == "Vec4")
            {

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 6;
                zSpinner.Increment = 0.1M;

                Label wLabel = new Label();
                wLabel.Text = "W";

                NumericUpDown wSpinner = new NumericUpDown();
                wSpinner.DecimalPlaces = 6;
                wSpinner.Increment = 0.1M;

                groupBoxFlowLayout.Controls.Add(xLabel);
                groupBoxFlowLayout.Controls.Add(xSpinner);

                groupBoxFlowLayout.Controls.Add(yLabel);
                groupBoxFlowLayout.Controls.Add(ySpinner);

                groupBoxFlowLayout.Controls.Add(zLabel);
                groupBoxFlowLayout.Controls.Add(zSpinner);

                groupBoxFlowLayout.Controls.Add(wLabel);
                groupBoxFlowLayout.Controls.Add(wSpinner);

                this.Height = 250;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Form1.UpdateView();
                    }
                );

                wSpinner.ValueChanged += new EventHandler(
                   delegate(object sender, EventArgs e)
                   {
                       Form1.UpdateView();
                   }
               );
            }
            else if (Type == "Texture2D")
            {
                // todo:
            }
            else if (Type == "TextureCube")
            { 
                // todo:
            }
        }
    }
}
