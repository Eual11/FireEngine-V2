    #version 330 core
    out vec4 FragColor;
    in vec4 vertColor;
    uniform float u_time;
  
    void main()
    {
      FragColor = sin(u_time)/2.0f+0.5*vertColor;
    };
