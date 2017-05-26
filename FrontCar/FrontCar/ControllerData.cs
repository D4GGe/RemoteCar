using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace FrontCar
{
    public class ControllerData
    {
        public Pad LeftPad { get; set; }
        public Pad RightPad { get; set; }

        public int LeftTrigger { get; set; }

        public int RightTrigger { get; set; }
    }
}
