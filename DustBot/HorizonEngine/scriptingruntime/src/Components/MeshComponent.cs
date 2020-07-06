using System.Collections.Generic;
using HorizonEngine.Core;
using HorizonEngine.Structures;

namespace HorizonEngine.Components
{
    public class MeshComponent : Component
    {
        public MeshComponent(string meshResourceId)
        {
            m_attributes.Add(new ComponentAttribute("meshResourceId", meshResourceId)); 
        }

        

    }
}