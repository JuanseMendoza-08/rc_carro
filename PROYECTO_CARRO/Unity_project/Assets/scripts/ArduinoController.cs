using UnityEngine;
using System.IO.Ports;
using System.Collections;

public class ArduinoController : MonoBehaviour
{
    public string portName = "COM5";
    public int baudRate = 9600;

    private SerialPort serialPort;
    private string status = "Disconnected";

    void Start()
    {
        ConnectArduino();
    }

    void OnGUI()
    {
        GUI.Label(new Rect(20, 20, 400, 30), "Status: " + status);
        GUI.Label(new Rect(20, 50, 400, 30), "Port: " + portName);

        if (GUI.Button(new Rect(20, 100, 180, 50), "Connect"))
        {
            ConnectArduino();
        }

        if (GUI.Button(new Rect(220, 100, 180, 50), "Automatic Mode"))
        {
            SendCommand("M");
        }

        if (GUI.Button(new Rect(420, 100, 180, 50), "RC Mode"))
        {
            SendCommand("R");
        }

        if (GUI.Button(new Rect(220, 180, 180, 60), "Forward"))
        {
            SendCommand("F");
        }

        if (GUI.Button(new Rect(220, 320, 180, 60), "Backward"))
        {
            SendCommand("T");
        }

        if (GUI.Button(new Rect(20, 250, 180, 60), "Left"))
        {
            SendCommand("L");
        }

        if (GUI.Button(new Rect(420, 250, 180, 60), "Right"))
        {
            SendCommand("D");
        }

        if (GUI.Button(new Rect(220, 250, 180, 60), "Stop"))
        {
            SendCommand("S");
        }
    }

    public void ConnectArduino()
    {
        try
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Close();
            }

            serialPort = new SerialPort(portName, baudRate);
            serialPort.ReadTimeout = 100;
            serialPort.WriteTimeout = 100;
            serialPort.Open();

            status = "Connected";
            StartCoroutine(ActivateRCMode());
        }
        catch (System.Exception e)
        {
            status = "Error: " + e.Message;
        }
    }

    IEnumerator ActivateRCMode()
    {
        yield return new WaitForSeconds(2f);
        SendCommand("R");
    }

    public void SendCommand(string command)
    {
        try
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Write(command);
                status = "Sent: " + command;
            }
            else
            {
                status = "Port not open";
            }
        }
        catch (System.Exception e)
        {
            status = "Send error: " + e.Message;
        }
    }

    void OnApplicationQuit()
    {
        try
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Write("S");
                serialPort.Close();
            }
        }
        catch
        {
        }
    }
}