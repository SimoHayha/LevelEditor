using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ModelView
{
    public class PerfViewModel : ToolViewModel
    {
        private Model.Perf _perf = null;
        public Model.Perf Perf
        {
            get
            {
                return _perf;
            }
            set
            {
                if (value != _perf)
                    _perf = value;
            }
        }

        public PerfViewModel() :
            base("Perf")
        {
            ContentId = ToolContentId;
        }

        public const string ToolContentId = "PerfTool";

        public void OnLoad(Model.Perf perf)
        {
            this.Perf = perf;
        }
    }
}
