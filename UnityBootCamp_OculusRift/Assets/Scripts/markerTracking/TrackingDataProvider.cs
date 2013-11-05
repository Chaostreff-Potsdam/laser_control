using UnityEngine;

using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;

using Mono.Zeroconf;

public class TrackingDataProvider
{
	private System.Net.Sockets.Socket trackingServer;
	private byte[] currentTrackingData;
	private bool ready = false;
	
	~TrackingDataProvider()
	{
		trackingServer.Close();
	}
	
	public void run()
	{
		currentTrackingData = new byte[34];
		setupZeroconfSocket();
		
		while (true) updateTrackingData();
	}
	
	public byte[] latestData()
	{
		if (ready) lock (currentTrackingData)
		{ 
			return currentTrackingData;
		}
		
		return new byte[34];
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
		trackingServer = new Socket(System.Net.Sockets.AddressFamily.InterNetwork, 
			System.Net.Sockets.SocketType.Stream, 
			System.Net.Sockets.ProtocolType.Tcp);
		trackingServer.Connect(endPoint);
		if (trackingServer.Connected) 
		{
			ready = true;
			UnityEngine.Debug.Log("MarkerTracking connected!");
		}
		
	}

	private void updateTrackingData()
	{
		if (trackingServer == null)
			return;
		
		if (!trackingServer.Connected)
			return;
		
		//lock (currentTrackingData)
		{
			trackingServer.Receive(currentTrackingData);
		}
	}
}