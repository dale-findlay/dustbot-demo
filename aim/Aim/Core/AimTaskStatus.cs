using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Aim.Core
{
    public enum AimTaskStatus
    {
        Success,
        Failure,
        Running
    }

    public static class AimTaskStatusHelper
    {
        public static AimTaskStatus FromBoolean(bool b)
        {
            if (b == true)
            {
                return AimTaskStatus.Success;
            }
            else
            {
                return AimTaskStatus.Failure;
            }
        }
    }
}
