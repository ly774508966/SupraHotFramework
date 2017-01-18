using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using SupraHot;
using SupraHot.CLI;

namespace SupraHotEditor
{
    class TransformComponentView : ComponentView
    {
        private TransformComponentCLI TransformComponent;
        private FlowLayoutPanel layoutPanel;

        private NumericUpDown XGlobalSpinner;
        private NumericUpDown YGlobalSpinner;
        private NumericUpDown ZGlobalSpinner;

        private NumericUpDown XLocalSpinner;
        private NumericUpDown YLocalSpinner;
        private NumericUpDown ZLocalSpinner;

        public TransformComponentView(TransformComponentCLI transformComponentCLI)
        {
            TransformComponent = transformComponentCLI;

            layoutPanel = new FlowLayoutPanel();
            layoutPanel.Dock = DockStyle.Fill;

            this.Width = 200;
            this.Height = 925;

            Color lightGrey = Color.FromArgb(40, 40, 40);
            Color whiteColor = Color.FromArgb(255, 255, 255);
            this.BackColor = lightGrey;
            this.ForeColor = whiteColor;

            Controls.Add(layoutPanel);

            Label description = new Label();
            description.Text = "Transform";
            layoutPanel.Controls.Add(description);


            // Add Position
            {
                Label spinnerBoxDescription = new Label();
                spinnerBoxDescription.Text = "Position";
                layoutPanel.Controls.Add(spinnerBoxDescription);

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 12;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 12;
                ySpinner.Increment = 0.1M;
                ySpinner.Maximum = (decimal)10000000000000.0f;
                ySpinner.Minimum = -ySpinner.Maximum;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 12;
                zSpinner.Increment = 0.1M;
                zSpinner.Maximum = (decimal)10000000000000.0f;
                zSpinner.Minimum = -zSpinner.Maximum;

                layoutPanel.Controls.Add(xLabel);
                layoutPanel.Controls.Add(xSpinner);

                layoutPanel.Controls.Add(yLabel);
                layoutPanel.Controls.Add(ySpinner);
                
                layoutPanel.Controls.Add(zLabel);
                layoutPanel.Controls.Add(zSpinner);

                Vec3CLI val = TransformComponent.GetLocalPosition();
                xSpinner.Value = (decimal)val.x;
                ySpinner.Value = (decimal)val.y;
                zSpinner.Value = (decimal)val.z;

                Console.WriteLine("Init Transform Component view Position to [{0}, {1}, {2}]", val.x, val.y, val.z);

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetLocalPosition(result);
                        UpdateGlobalLocalPosition();
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetLocalPosition(result);
                        UpdateGlobalLocalPosition();
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetLocalPosition(result);
                        UpdateGlobalLocalPosition();
                        Form1.UpdateView();
                    }
                );

                XLocalSpinner = xSpinner;
                YLocalSpinner = ySpinner;
                ZLocalSpinner = zSpinner;
            }

            // Add Rotation (ZYX Ordering!)
            {
                Label spinnerBoxDescription = new Label();
                spinnerBoxDescription.Text = "Rotation";
                layoutPanel.Controls.Add(spinnerBoxDescription);

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;
                ySpinner.Maximum = (decimal)10000000000000.0f;
                ySpinner.Minimum = -ySpinner.Maximum;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 6;
                zSpinner.Increment = 0.1M;
                zSpinner.Maximum = (decimal)10000000000000.0f;
                zSpinner.Minimum = -zSpinner.Maximum;

                layoutPanel.Controls.Add(xLabel);
                layoutPanel.Controls.Add(xSpinner);

                layoutPanel.Controls.Add(yLabel);
                layoutPanel.Controls.Add(ySpinner);

                layoutPanel.Controls.Add(zLabel);
                layoutPanel.Controls.Add(zSpinner);

                Vec3CLI val = TransformComponent.GetRotation();
                xSpinner.Value = (decimal)val.x;
                ySpinner.Value = (decimal)val.y;
                zSpinner.Value = (decimal)val.z;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetRotation(result);
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetRotation(result);
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetRotation(result);
                        Form1.UpdateView();
                    }
                );
            }

            // Add Scale 
            {
                Label spinnerBoxDescription = new Label();
                spinnerBoxDescription.Text = "Local Scale";
                layoutPanel.Controls.Add(spinnerBoxDescription);

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;
                ySpinner.Maximum = (decimal)10000000000000.0f;
                ySpinner.Minimum = -ySpinner.Maximum;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 6;
                zSpinner.Increment = 0.1M;
                zSpinner.Maximum = (decimal)10000000000000.0f;
                zSpinner.Minimum = -zSpinner.Maximum;

                layoutPanel.Controls.Add(xLabel);
                layoutPanel.Controls.Add(xSpinner);

                layoutPanel.Controls.Add(yLabel);
                layoutPanel.Controls.Add(ySpinner);

                layoutPanel.Controls.Add(zLabel);
                layoutPanel.Controls.Add(zSpinner);

                Vec3CLI val = TransformComponent.GetScale();
                xSpinner.Value = (decimal)val.x;
                ySpinner.Value = (decimal)val.y;
                zSpinner.Value = (decimal)val.z;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetScale(result);
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetScale(result);
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetScale(result);
                        Form1.UpdateView();
                    }
                );
            }

            // Add Global Scale 
            {
                Label spinnerBoxDescription = new Label();
                spinnerBoxDescription.Text = "Global Scale";
                layoutPanel.Controls.Add(spinnerBoxDescription);

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;
                ySpinner.Maximum = (decimal)10000000000000.0f;
                ySpinner.Minimum = -ySpinner.Maximum;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 6;
                zSpinner.Increment = 0.1M;
                zSpinner.Maximum = (decimal)10000000000000.0f;
                zSpinner.Minimum = -zSpinner.Maximum;

                layoutPanel.Controls.Add(xLabel);
                layoutPanel.Controls.Add(xSpinner);

                layoutPanel.Controls.Add(yLabel);
                layoutPanel.Controls.Add(ySpinner);

                layoutPanel.Controls.Add(zLabel);
                layoutPanel.Controls.Add(zSpinner);

                Vec3CLI val = TransformComponent.GetGlobalScale();
                xSpinner.Value = (decimal)val.x;
                ySpinner.Value = (decimal)val.y;
                zSpinner.Value = (decimal)val.z;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetGlobalScale(result);
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetGlobalScale(result);
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetGlobalScale(result);
                        Form1.UpdateView();
                    }
                );
            }

           
            // Add Global Position 
            {
                Label spinnerBoxDescription = new Label();
                spinnerBoxDescription.Text = "Global Position";
                layoutPanel.Controls.Add(spinnerBoxDescription);

                Label xLabel = new Label();
                xLabel.Text = "X";

                NumericUpDown xSpinner = new NumericUpDown();
                xSpinner.DecimalPlaces = 6;
                xSpinner.Increment = 0.1M;
                xSpinner.Maximum = (decimal)10000000000000.0f;
                xSpinner.Minimum = -xSpinner.Maximum;

                Label yLabel = new Label();
                yLabel.Text = "Y";

                NumericUpDown ySpinner = new NumericUpDown();
                ySpinner.DecimalPlaces = 6;
                ySpinner.Increment = 0.1M;
                ySpinner.Maximum = (decimal)10000000000000.0f;
                ySpinner.Minimum = -ySpinner.Maximum;

                Label zLabel = new Label();
                zLabel.Text = "Z";

                NumericUpDown zSpinner = new NumericUpDown();
                zSpinner.DecimalPlaces = 6;
                zSpinner.Increment = 0.1M;
                zSpinner.Maximum = (decimal)10000000000000.0f;
                zSpinner.Minimum = -zSpinner.Maximum;

                layoutPanel.Controls.Add(xLabel);
                layoutPanel.Controls.Add(xSpinner);

                layoutPanel.Controls.Add(yLabel);
                layoutPanel.Controls.Add(ySpinner);

                layoutPanel.Controls.Add(zLabel);
                layoutPanel.Controls.Add(zSpinner);

                Vec3CLI val = TransformComponent.GetGlobalPosition();
                xSpinner.Value = (decimal)val.x;
                ySpinner.Value = (decimal)val.y;
                zSpinner.Value = (decimal)val.z;

                // Add listeners
                xSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetGlobalPosition(result);
                        UpdateGlobalLocalPosition();
                        Form1.UpdateView();
                    }
                );

                ySpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetGlobalPosition(result);
                        UpdateGlobalLocalPosition();
                        Form1.UpdateView();
                    }
                );

                zSpinner.ValueChanged += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        Vec3CLI result = new Vec3CLI((float)xSpinner.Value, (float)ySpinner.Value, (float)zSpinner.Value);
                        TransformComponent.SetGlobalPosition(result);
                        UpdateGlobalLocalPosition();
                        Form1.UpdateView();
                    }
                );

                Button setGlob = new Button();
                setGlob.Text = "Reset global";
                layoutPanel.Controls.Add(setGlob);

                setGlob.Click += new EventHandler(
                        delegate(object sender, EventArgs e)
                        {
                            TransformComponent.SetGlobalPosition(new Vec3CLI(0, 0, 0));
                            UpdateGlobalLocalPosition();
                            Form1.UpdateView();
                        }
                    );

                XGlobalSpinner = xSpinner;
                YGlobalSpinner = ySpinner;
                ZGlobalSpinner = zSpinner;
            }
        }

        void UpdateGlobalLocalPosition() 
        {
            Vec3CLI globalPosition = TransformComponent.GetGlobalPosition();
            Vec3CLI localPosition = TransformComponent.GetLocalPosition();

            // Update Local Position
            XLocalSpinner.Value = (decimal)localPosition.x;
            YLocalSpinner.Value = (decimal)localPosition.y;
            ZLocalSpinner.Value = (decimal)localPosition.z;

            // Update Global Position
            XGlobalSpinner.Value = (decimal)globalPosition.x;
            YGlobalSpinner.Value = (decimal)globalPosition.y;
            ZGlobalSpinner.Value = (decimal)globalPosition.z;
        }
    }
}
