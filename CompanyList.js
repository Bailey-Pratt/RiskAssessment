import React, { useState, useEffect } from 'react';

const CompanyList = () => {
  const [companies, setCompanies] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    const fetchCompanies = async () => {
      try {
        setLoading(true);
        const response = await fetch('http://localhost:8080/companies');
        const data = await response.json();
        
        if (data.status === 'success') {
          setCompanies(data.companies);
        } else {
          setError('Failed to load companies');
        }
      } catch (err) {
        setError('Error connecting to server');
        console.error('Error fetching companies:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchCompanies();
  }, []);

  if (loading) {
    return <div className="company-list-loading">Loading...</div>;
  }

  if (error) {
    return null; // Don't show anything if there's an error
  }

  return (
    <div className="company-list-container">
      <h3 className="company-list-title">Companies using our tool:</h3>
      {companies.length > 0 ? (
        <ul className="company-list">
          {companies.map((company, index) => (
            <li key={index} className="company-list-item">{company}</li>
          ))}
        </ul>
      ) : (
        <p className="company-list-empty">No companies registered yet</p>
      )}
    </div>
  );
};

export default CompanyList;