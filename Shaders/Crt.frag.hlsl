// Sampler
sampler2D TexSampler : register(s0);

// TextureSize
float2 TextureSize : register(c0);

// Brightness factor for darker scanlines
float BrightnessFactorRow2 : register(c1);
float BrightnessFactorRow3 : register(c2);

// Overspill from the primary colors
float Overspill : register(c3);

// Diagonal or vertical raster
float Diagonal : register(c4);

float4 main(float2 texCoord : TEXCOORD) : SV_Target
{    
    // Scale to int texture size. Row and col are the current coordinates in the bitmap from
    // the upper left corner.
    int row = (int)(texCoord.y * TextureSize.y);
    int col = (int)(texCoord.x * TextureSize.x);

    // Pick up the color at the current position and add some brightness.
    float4 color = tex2D(TexSampler, texCoord) + 0.1f;

    float4 outColor   = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 multiplier = float4(0.0f, 0.0f, 0.0f, 1.0f);

    // Get the pixel position within a 3 x 3 matrix.
    int rowCheck = row % 3;
    int colCheck = col % 3;

    // The pixel color is handled by setting a R/G/B multiplier vector.
    // First check if a diagonal raster should be implemented.
    if (Diagonal == 1)
    {
        // Process the pixels, shifting the colors one step to the right for every row
        // within the 3 x 3 matrix.
        if (rowCheck == colCheck)
        {
            multiplier.r = 1.0f; 
            multiplier.g = Overspill;
            multiplier.b = Overspill;
        }
        else if ((rowCheck == 0 && colCheck == 1) || (rowCheck == 1 && colCheck == 2) || (rowCheck == 2 && colCheck == 0))
        {
            multiplier.g = 1.0f; 
            multiplier.b = Overspill;
            multiplier.r = Overspill;
        }
        else
        {
            multiplier.b = 1.0f; 
            multiplier.r = Overspill;
            multiplier.g = Overspill;
        }
    }
    else
    {
        // For a vertical raster, process pixels without shifting.
        if (colCheck == 0)
        {
            multiplier.r = 1.0f; 
            multiplier.g = Overspill;
            multiplier.b = Overspill;
        }
        else if (colCheck == 1)
        {
            multiplier.g = 1.0f; 
            multiplier.b = Overspill;
            multiplier.r = Overspill;
        }
        else
        {
            multiplier.b = 1.0f; 
            multiplier.r = Overspill;
            multiplier.g = Overspill;
        }
    }

    // Add scanlines.
    if (rowCheck == 1)
    {
        // Make the second of the three rows a bit darker to simulate a scan line.
        multiplier *= BrightnessFactorRow2;
    }

    if (rowCheck == 2)
    {
        // Make the last of the three rows a bit darker to simulate a scan line.
        multiplier *= BrightnessFactorRow3;
    }

    // Apply the multiplier to set the final color.
    outColor = color * multiplier;

    // The Alpha channel needs to be restored to 1 after all operations.
    outColor.a = 1.0f;

    return outColor;
}
