import React, { Component } from 'react';
import { connect } from 'react-redux';
import { Link } from 'react-router-dom';
import { login } from 'actions/auth';
import UserForm from 'components/shared/UserForm';

import 'styles/Login.css';

const errorMessages = {
    email: 'This email address is not registered!'
};

class Login extends Component {
    static path = '/login';

    onSubmit = (email, password) => {
        const { login, history } = this.props;
        login(email, password)
            .then(next => {
                if (next) history.push('/');
            });
    }

    render() {
        const error = !!this.props.error

        return (
            <div className='Login'>
                <p>
                    New to Shekel Tracker?
                    <Link to={'/register'}> Create an account. </Link>
                </p>
                <UserForm
                    title='Login'
                    isLoading={this.props.isLoading}
                    onSubmit={this.onSubmit}
                    submitText='Sign In'
                />
                {error ? <p className='errorMsg'>{errorMessages.email}</p> : ''}
            </div>
        );
    }
}

const mapStateToProps = state => ({
    isLoading: state.auth.isLoading,
    error: state.auth.error
});

const mapDispatchToProps = dispatch => ({
    login: (...userData) => dispatch(login(...userData))
});

export default connect(mapStateToProps, mapDispatchToProps)(Login);
