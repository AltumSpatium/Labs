import React, { Component } from 'react';
import { Form, Checkbox, Button, Loader, Dimmer, Segment, Header } from 'semantic-ui-react';

// TODO: Improve validation

const errorMessages = {
    email: 'Email field must contain a valid email address!',
    password: 'Password must be at least 6 characters long!',
    empty: 'This field must be filled!'
};

class UserForm extends Component {
    constructor(props) {
        super(props);

        this.state = {
            email: '',
            password: '',
            isAgree: false,
            emailError: '',
            passwordError: ''
        };
    }

    checkValidity(name, value) {
        let { emailError, passwordError } = this.state;
        switch (name) {
            case 'email':
                if (!value) emailError = errorMessages.empty;
                else if (!/^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(value)) emailError = errorMessages.email;
                else emailError = '';
                setTimeout(() => this.setState({emailError}), 500);
                break;
            case 'password':
                if (!value) passwordError = errorMessages.empty;
                else if (value.length < 6) passwordError = errorMessages.password;
                else passwordError = '';
                setTimeout(() => this.setState({passwordError}), 500);
                break;
            default:
                break;
        }
    }

    onSubmit = (e) => {
        e.preventDefault();
        const { email, password } = this.state;
        const { emailError, passwordError } = this.state;

        this.checkValidity('email', email);
        this.checkValidity('password', password);
        if (email && password && !emailError && !passwordError)
            this.props.onSubmit(email, password);
    }

    onChange = (e) => {
        const name = e.target.name;
        const value = e.target.value;
        this.setState({ [name]: value });

        this.checkValidity(name, value);
    }

    isAgreeHandler = () => this.setState({ isAgree: !this.state.isAgree });

    render() {
        const { email, password, isAgree } = this.state;
        const { isLoading, title, submitText } = this.props;

        return (
            <Dimmer.Dimmable as={Segment} dimmed={isLoading} className='user-form'>
                <Dimmer active={isLoading} inverted >
                    <Loader active={isLoading}> Wait, please </Loader>
                </Dimmer>

                <Form>
                    <Header as='h3' textAlign='center'>{title}</Header>
                    <Form.Input
                        label='Email'
                        placeholder='Email'
                        name='email'
                        value={email}
                        onChange={this.onChange}
                        error={!!this.state.emailError}
                    />
                    {this.state.emailError ? <p className='errorMsg'>{this.state.emailError}</p> : ''}
                    <Form.Input
                        label='Password'
                        placeholder='Password'
                        name='password'
                        value={password}
                        type='password'
                        onChange={this.onChange}
                        error={!!this.state.passwordError}
                    />
                    {this.state.passwordError ? <p className='errorMsg'>{this.state.passwordError}</p> : ''}
                    {
                        title === 'Registration' ? (
                            <Form.Field>
                                <Checkbox
                                    checked={isAgree}
                                    onChange={this.isAgreeHandler}
                                    label='I agree to the Terms and Conditions'
                                />
                            </Form.Field>
                        ) : ('')
                    }
                    <Button
                        fluid positive
                        content={submitText}
                        disabled={title === 'Registration' ? !isAgree : false}
                        onClick={this.onSubmit}
                    />
                </Form>
            </Dimmer.Dimmable>
        );
    }
}

export default UserForm;
