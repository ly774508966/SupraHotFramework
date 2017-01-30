import System
import clr
clr.AddReference('System.Windows.Forms')
from System.Windows.Forms import *

form = Form()
form.Text = "Wurst"
form.Show()
	
print "OK"
	
print "Ok"

from SupraHot import *
from SupraHot.CLI import *
from SupraHotEditor import *
	
xx = Vec3CLI()
xx.Set(1, 2, 3)
xx.Print()