using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ModelView
{
    interface IDragable
    {
        Type DataType
        {
            get;
        }

        void Remove(Object i);
    }
}
