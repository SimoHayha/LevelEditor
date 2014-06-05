using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ModelView
{
    public class ConsoleViewModel : ToolViewModel
    {
        private Model.Console _console = null;
        public Model.Console Console
        {
            get { return this._console; }
            set
            {
                if (value != _console)
                    _console = value;
            }
        }

        public ConsoleViewModel() :
            base("Console")
        {
            ContentId = ToolContentId;
        }

        public const string ToolContentId = "ConsoleTool";

        public void OnLoad(Model.Console Console)
        {
            this.Console = Console;
        }
    }
}
