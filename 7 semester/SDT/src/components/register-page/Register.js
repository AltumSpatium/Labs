import React, { Component } from 'react';
import { connect } from 'react-redux';
import { Link } from 'react-router-dom';
import { login, register } from 'actions/auth';
import UserForm from 'components/shared/UserForm';

import 'styles/Register.css';

const errorMessages = {
    email: 'This email address is already taken!'
};

class Register extends Component {
    static path = '/register';

    onSubmit = (email, password) => {
        const { register, history } = this.props;
        register(email, password)
            .then(user => {
                if (user) return this.props.login(email, password)
            })
            .then(next => {
                if (next) history.push('/')
            });
    }

    render() {
        const error = !!this.props.error

        return (
            <div className="Register">
                <p>
                    Alredy have an account?
                    <Link to={'/login'}> Log In. </Link>
                </p>
                <UserForm
                    title='Registration'
                    isLoading={this.props.isLoading}
                    onSubmit={this.onSubmit}
                    submitText='Sign Up'
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
    register: (...userData) => dispatch(register(...userData)),
    login: (...userData) => dispatch(login(...userData)),
});

export default connect(mapStateToProps, mapDispatchToProps)(Register);
