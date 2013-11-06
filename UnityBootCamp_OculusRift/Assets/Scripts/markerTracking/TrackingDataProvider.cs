using UnityEngine;

using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Drawing;

using Mono.Zeroconf;

public class TrackingDataProvider
{
	private System.Net.Sockets.Socket m_trackingServer;
	private byte[] m_currentTrackingData;
	private bool m_ready = false;
	private IDictionary<byte, PointF> m_markerPositions;
	
	public TrackingDataProvider()
	{
		m_markerPositions = new Dictionary<byte, PointF>();
		m_markerPositions[1] = new PointF(0f, 0f);
	}
	
	~TrackingDataProvider()
	{
		m_trackingServer.Close();
	}
	
	public void run()
	{
		m_currentTrackingData = new byte[34];
		setupZeroconfSocket();
		
		while (true) updateTrackingData();
	}
	
	public byte[] latestData()
	{
		if (m_ready) lock (m_currentTrackingData)
		{ 
			return m_currentTrackingData;
		}
		
		return new byte[34];
	}
	
	public PointF markerPositionById(byte id)
	{
		if (!m_markerPositions.ContainsKey(1))
			UnityEngine.Debug.Log("Error: key not found");
			// make this safe...
		
		PointF position = new PointF();
		m_markerPositions.TryGetValue(id, out position);
		
		return position;
	}
	
	private void setupZeroconfSocket()
	{
		/*
		// connect to MarkerTracking service
		ServiceBrowser browser = new ServiceBrowser ();
 
		//
		// Configure the code that will be called back when the information
		// becomes available
		//
		browser.ServiceAdded += delegate (object o, ServiceBrowseEventArgs args) {
		    UnityEngine.Debug.Log ("Found Service: " + args.Service.Name);
		    args.Service.Resolved += delegate (object o2, ServiceResolvedEventArgs args2) {
		        IResolvableService s = (IResolvableService)args2.Service;
		        UnityEngine.Debug.Log ("Resolved Service: " + s.FullName + " - " + s.HostEntry.AddressList[0] + ":" + s.Port);
				
				// conntect socket
				IPEndPoint endPoint = new IPEndPoint(s.HostEntry.AddressList[0], 3100);
				trackingServer = new Socket(System.Net.Sockets.AddressFamily.InterNetwork, 
					System.Net.Sockets.SocketType.Stream, 
					System.Net.Sockets.ProtocolType.Tcp);
				trackingServer.Connect(endPoint);
				if (trackingServer.Connected) 
				{
					ready = true;
					UnityEngine.Debug.Log("Connected!");
				}
		    };
			
		    args.Service.Resolve ();
		};
	
		// 
		// Trigger the request
		//
		UnityEngine.Debug.Log("Looking for toptracking service...");
		browser.Browse ("_quantum-toptracking._tcp", "local");
		*/
		
		
		// conntect socket
		IPEndPoint endPoint = new IPEndPoint(new IPAddress(new byte[]{172, 16, 19, 188}), 3100);
		m_trackingServer = new Socket(System.Net.Sockets.AddressFamily.InterNetwork, 
			System.Net.Sockets.SocketType.Stream, 
			System.Net.Sockets.ProtocolType.Tcp);
		m_trackingServer.Connect(endPoint);
		if (m_trackingServer.Connected) 
		{
			m_ready = true;
			UnityEngine.Debug.Log("MarkerTracking connected!");
		}
		
	}

	private void updateTrackingData()
	{
		if (m_trackingServer == null)
			return;
		
		if (!m_trackingServer.Connected)
			return;
		
		// should we really not lock here?
		//
		byte[] newData = new byte[34];
		m_trackingServer.Receive(newData);
		updatePositions(newData);
		
	}
	
	private void updatePositions(byte[] data)
	{
		if (data.Length != 34)
		{
			UnityEngine.Debug.Log("TrackingDataProvider: Wrong data format");
			return;
		}
		
		byte id = data[9];
		float x = (float)BitConverter.ToDouble(data, 10);
		float z = (float)BitConverter.ToDouble(data, 18);
		m_markerPositions[id] = new PointF(x, z);
	}
		
}	