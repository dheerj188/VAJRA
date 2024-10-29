import Navbar from '../assets/components/Navbar'; // Adjust the path as necessary
import '../assets/css/Home.css';  // Import the custom CSS for grainy effect
import '../index.css'; // Make sure index.css is imported in your entry file
import { useState, useEffect } from "react";

const Home = () => {

  const [messageSent, setMessageSent] = useState(false);
  const [receivedMessage, setReceivedMessage] = useState(""); // Store the received message
  const [socket, setSocket] = useState(null); // WebSocket instance

  useEffect(() => {
    // Open WebSocket connection when the component mounts
    const server_ip = '10.0.2.15';  // Your WebSocket server IP
    const server_port = 8765;  // Your WebSocket server port
    const ws = new WebSocket(`ws://${server_ip}:${server_port}`);  // Create WebSocket connection
    // const ws = new WebSocket('ws://localhost:8765');

    // Handle connection open
    ws.onopen = () => {
      console.log('WebSocket connection opened');
      setSocket(ws);  // Save WebSocket instance to state
    };

    // Handle messages from the server
    ws.onmessage = (event) => {
      console.log('Message from server:', event.data);
      setReceivedMessage(event.data);  // Update the state with the received message
    };

    // Handle WebSocket closing
    ws.onclose = () => {
      console.log('WebSocket connection closed');
    };

    // Clean up the WebSocket connection when the component unmounts
    return () => {
      if (ws) {
        ws.close();
      }
    };
  }, []);

  // Function to send a message via WebSocket
  const sendMessage = () => {
    const message = 'Hello, Server';  // Customize the message here

    if (socket && socket.readyState === WebSocket.OPEN) {
      socket.send(message);  // Send message through WebSocket
      setMessageSent(true);
      console.log('Message sent:', message);
    } else {
      console.error('WebSocket is not connected or not ready');
    }
  };

  return (
    <div className="relative h-screen flex flex-col items-center">
      {/* Background image with grainy overlay */}
      <div className="absolute inset-0 bg-home-bg3 bg-cover bg-center opacity-100 grainy-overlay z-[-1]"></div>
      <Navbar />
      {/* Content section */}
      <div className="relative z-10 w-[79%] mx-auto mt-72 text-left">
        <h1 className="text-3xl md:text-5xl lg:text-7xl font-greek-excon text-white drop-shadow-lg">
          Versatile AI Junction for Real-time Edge Applications
        </h1>
        <p className="text-base md:text-lg lg:text-xl text-white mt-10">
          The user interface designed to streamline your deep learning and edge computing research.
        </p>
        <div className="flex space-x-10 mt-12">
          <button className="bg-gray-200 text-black px-6 py-3 w-48 rounded-xl font-semibold hover:bg-gray-300">
            Docs →
          </button>
          <button className="bg-gray-200 text-black px-6 py-3 w-48 rounded-xl font-semibold hover:bg-gray-300">
            Get Started →
          </button>
        </div>

        <div className="flex space-x-10 mt-12">
          <button 
            className="bg-gray-200 text-black px-6 py-3 w-48 rounded-xl font-semibold hover:bg-gray-300"
            onClick={sendMessage}
          >
            Send Message →
          </button>
        </div>

        {messageSent && <p className="text-green-500 mt-5">Message sent successfully!</p>}
        {receivedMessage && <p className="text-blue-500 mt-5">Received: {receivedMessage}</p>}
      </div>
    </div>
  );
};

export default Home;
