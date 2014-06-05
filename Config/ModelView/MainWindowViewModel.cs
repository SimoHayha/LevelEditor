using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Config.ModelView
{
    public class MainWindowViewModel
    {
        private Model.Model model_ = null;

        public Model.Model Model
        {
            get
            {
                return this.model_;
            }
            private set
            {
                this.model_ = value;
            }
        }

        public MainWindowViewModel()
        {
            this.Init();
        }

        public bool Init()
        {
            this.Model = new Model.Model();
            return true;
        }
    }
}
