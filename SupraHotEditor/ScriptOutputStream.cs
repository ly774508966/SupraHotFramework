using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.ComponentModel;
using System.Data;
using System.IO;


// Source : https://www.codeproject.com/Articles/755977/IronPython-A-Quick-WinForms-Introduction

namespace SupraHotEditor
{
    public class ScriptOutputStream : Stream
    {
        #region Fields
        private readonly TextBox _control;
        #endregion

        #region Constructors
        public ScriptOutputStream(TextBox control)
        {
            _control = control;
        }
        #endregion

        #region Properties
        public override bool CanRead
        {
            get { return false; }
        }

        public override bool CanSeek
        {
            get { return false; }
        }

        public override bool CanWrite
        {
            get { return true; }
        }

        public override long Length
        {
            get { throw new NotImplementedException(); }
        }

        public override long Position
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
        }
        #endregion

        #region Exposed Members
        public override void Flush()
        {
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            throw new NotImplementedException();
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            throw new NotImplementedException();
        }

        public override void SetLength(long value)
        {
            throw new NotImplementedException();
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            _control.Text += Encoding.GetEncoding(1252).GetString(buffer, offset, count);
        }
        #endregion
    }
}