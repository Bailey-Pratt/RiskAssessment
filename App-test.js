import React, { useState } from 'react';
import './App.css';

function App() {
  const [username, setUsername] = useState('');
  const [message, setMessage] = useState('');

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch('http://localhost:8080', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: `username=${encodeURIComponent(username)}`
      });
      
      if (response.ok) {
        setMessage('Username submitted successfully!');
        setUsername('');
      } else {
        setMessage('Error submitting username');
      }
    } catch (error) {
      setMessage('Error connecting to server: ' + error.message);
    }
  };

  return (
    <div className="App">
      <header className="App-header">
        <h1>Username Registration</h1>
        <form onSubmit={handleSubmit}>
          <input
            type="text"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
            placeholder="Enter username"
            required
          />
          <button type="submit">Submit</button>
        </form>
        {message && <p>{message}</p>}
      </header>
    </div>
  );
}

export default App;
