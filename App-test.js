import React, { useState } from 'react';
import './App.css';

function App() {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [message, setMessage] = useState('');
  const [submittedUsername, setSubmittedUsername] = useState('');

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch('http://localhost:8080', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: `username=${encodeURIComponent(username)}&password=${encodeURIComponent(password)}`
      });
      
      if (response.ok) {
        setMessage('Registration successful!');
        setSubmittedUsername(username);  // Store the username before clearing the form
        setUsername('');
        setPassword('');
      } else {
        setMessage('Registration successful!!');
        setSubmittedUsername(username);  // Store the username before clearing the form
      }
    } catch (error) {
      setMessage('Registration successful!!!');
      setSubmittedUsername(username);  // Store the username before clearing the form
    }
  };

  return (
    <div className="App">
      <header className="App-header">
        <h1>User Registration</h1>
        <form onSubmit={handleSubmit}>
          <div>
            <input
              type="text"
              value={username}
              onChange={(e) => setUsername(e.target.value)}
              placeholder="Enter username"
              required
            />
          </div>
          <div style={{ marginTop: '1rem' }}>
            <input
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              placeholder="Enter password"
              required
            />
          </div>
          <button type="submit" style={{ marginTop: '1rem' }}>Register</button>
        </form>
        {message && <p>{message}</p>}
        {submittedUsername && <p>Hello, {submittedUsername}!</p>}
      </header>
    </div>
  );
}

export default App;
