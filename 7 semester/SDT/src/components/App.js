import React, { Component } from 'react';
import PropTypes from 'prop-types';
import { Link } from 'react-router-dom';
import { connect } from 'react-redux';
import { Menu, Button } from 'semantic-ui-react';
import { logout } from 'actions/auth';
import { linkSet } from 'constants/default';

import 'styles/App.css';

class App extends Component {
    static path = '/';
    static contextTypes = {
        router: PropTypes.object.isRequired
    };

    constructor() {
        super();

        this.logout = this.logout.bind(this);
    }

    logout() {
        this.props.logout();
        this.context.router.history.push('/');
    }

    render() {
        const user = localStorage['stUser'] ? JSON.parse(localStorage['stUser']) : null;
        const links = user ? linkSet.USERS_LINKS : linkSet.GUESTS_LINKS;

        return (
            <div className='App'>
                <Menu>
                    <Menu.Item header>Shekel Tracker</Menu.Item>
                    {
                        links.map((link, index) => (
                            <Link to={link.path} key={index}>
                                <Menu.Item name={link.name}>{link.name}</Menu.Item>
                            </Link>
                        ))
                    }
                    {
                        user ? (
                            <Menu.Menu position='right'>
                                <Button
                                    negative
                                    color='teal'
                                    icon='log out'
                                    className='logout-button'
                                    onClick={this.logout}
                                />
                            </Menu.Menu>
                        ) : ''
                    }

                </Menu>
                {this.props.children}
            </div>
        );
    }
}

const mapStateToProps = state => ({
    isLoading: state.auth.isLoading
});

const mapDispatchToProps = dispatch => ({
    logout: () => dispatch(logout()),
});

export default connect(mapStateToProps, mapDispatchToProps)(App);
