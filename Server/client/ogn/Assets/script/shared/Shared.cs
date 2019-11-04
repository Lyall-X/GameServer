

public class Shared
{
    public static int MAX_PACKET_LENGTH = 4096;
    public static void XOR(byte[] bytes, int len, string key)
    {
        if (key.Length == 0) return;
        byte [] keybytes = System.Text.Encoding.Default.GetBytes(key);
        int keylen = keybytes.Length;
        int keypos = 0;
        for (int i = 0; i < len; ++i)
        {
            bytes[i] ^= keylen > 0 ? keybytes[keypos] : (byte)0;
            keypos = (keypos +1) % keylen;
        }
    }
}
