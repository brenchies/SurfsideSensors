using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AQS_DEMO
{
    public partial class AQS_Demo_Form : Form
    {
        public AQS_Demo_Form()
        {
            InitializeComponent();
            //serialPortAQS.Open();
        }

        String portMessage;

        private void timer1_Tick(object sender, EventArgs e)
        {
            portMessage = serialPortAQS.ReadLine().Trim();
        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        private void AQS_Demo_Form_Load(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void panel9_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label13_Click(object sender, EventArgs e)
        {

        }
    }
}
