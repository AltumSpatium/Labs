import React, { Component } from 'react';
import { Header, Button, Modal } from 'semantic-ui-react';
import ReactTable from 'react-table';
import { tableHeaders } from 'constants/default';
import ActionsButtons from 'components/shared/ActionsButtons';
import RecordWindow from 'components/shared/RecordWindow';
import firebaseApp from 'utils/firebase';
import convertCurrency from 'utils/convertCurrency';
import toggleModalWindow from 'utils/toggleModalWindow';
import { connect } from 'react-redux';
import {
    getAllExpenses,
    addExpense,
    updateExpense,
    removeExpense,
    clearExpenses
} from 'actions/expenses';

import 'styles/Expenses.css';

// TODO: Add currency convertion support, add processing of planned expenses

const user = localStorage['stUser'] ? JSON.parse(localStorage['stUser']) : {}; // FIX DAT

class Expenses extends Component {
    static path = '/expenses';

    constructor(props) {
        super(props);

        this.state = {
            addWindow: false,
            editWindow: false,
            removeWindow: false,
            expenseId: null
        };

        this.expensesRef = firebaseApp.database().ref('expenses/' + user.uid);
        this.accountsRef = firebaseApp.database().ref('accounts/' + user.uid);

        this.toggleModalWindow = toggleModalWindow.bind(this);
        this.addExpense = this.addExpense.bind(this);
        this.editExpense = this.editExpense.bind(this);
        this.removeExpense = this.removeExpense.bind(this);
    }

    componentWillMount() {
        let newItems = false;
        this.expensesRef.on('child_added', snapshot => {
            if (!newItems) return;

            const newExpense = {
                id: snapshot.key,
                ...snapshot.val()
            };

            this.accountsRef.child(newExpense.account).once('value', s => {
                const account = s.val();
                account.money = +account.money - 
                    convertCurrency(newExpense.currency, account.currency, +newExpense.money);
                this.accountsRef.child(newExpense.account).update(account);
                this.props.addExpense(newExpense);
            });
        });
        this.expensesRef.on('child_changed', snapshot => {
            const updatedExpense = {
                id: snapshot.key,
                ...snapshot.val()
            };
            const oldExpense = this.props.expenses.filter(item => item.id === updatedExpense.id)[0];
            if (!oldExpense) return;

            this.accountsRef.child(oldExpense.account).once('value', s => {
                const account = s.val();
                account.money = +account.money +
                    convertCurrency(oldExpense.currency, account.currency, +oldExpense.money) -
                    convertCurrency(updatedExpense.currency, account.currency, +updatedExpense.money);
                this.accountsRef.child(oldExpense.account).update(account);
                this.props.updateIncome(updatedExpense);
            });
        });
        this.expensesRef.on('child_removed', snapshot => {
            const removedExpense = snapshot.val();

            this.accountsRef.child(removedExpense.account).once('value', s => {
                const account = s.val();
                if (account) {
                    account.money = +account.money +
                        convertCurrency(removedExpense.currency, account.currency, +removedExpense.money);
                    this.accountsRef.child(removedExpense.account).update(account);
                }
                this.props.removeExpense(snapshot.key);
            });
        });
        this.props.getAllExpenses()
            .then(() => { newItems = true });
    }

    componentWillUnmount() {
        this.expensesRef.off();
        this.props.clearExpenses();
    }

    addExpense(newExpense) {
        this.expensesRef.push(newExpense);
        this.toggleModalWindow('addWindow', 'expenseId');
    }

    editExpense(income) {
        const id = this.state.expenseId;
        this.expensesRef.child(id).update(income);
        this.toggleModalWindow('editWindow', 'expenseId');
    }

    removeExpense() {
        const id = this.state.expenseId;
        this.expensesRef.child(id).remove();
        this.toggleModalWindow('removeWindow', 'expenseId');
    }

    render() {
        const expenses = this.props.expenses.map(item => {
            item.displayMoney = `${item.money} ${item.currency}`;
            item.actions = (
                <ActionsButtons 
                    onEditClick={() => this.toggleModalWindow('editWindow', 'expenseId', item.id)}
                    onRemoveClick={() => this.toggleModalWindow('removeWindow', 'expenseId', item.id)} />
            );
            return item;
        });
        const getExpense = id => {
            return expenses.filter(item => item.id === id)[0];
        };

        return (
            <div className='Expenses'>
                <Header as='h1' textAlign='center'>Expenses</Header>
                <Button
                    positive
                    className='add-record-button'
                    onClick={() => this.toggleModalWindow('addWindow', 'expenseId')}>Add New Record</Button>
                <ReactTable
                    data={expenses}
                    columns={tableHeaders.EXPENSES}
                    defaultPageSize={10}
                    className='-striped -highlight'
                />

                <RecordWindow 
                    isOpen={this.state.addWindow}
                    headerText='Add new record'
                    submitText='Add'
                    onSubmit={this.addExpense}
                    onCancel={() => this.toggleModalWindow('addWindow', 'expenseId')}
                    recordType='expenses'
                    allowFutureDate={false}
                />

                <RecordWindow 
                    isOpen={this.state.editWindow}
                    headerText='Edit expense'
                    submitText='Edit'
                    onSubmit={this.editExpense}
                    onCancel={() => this.toggleModalWindow('editWindow', 'expenseId')}
                    recordType='expenses'
                    allowFutureDate={false}
                    values={getExpense(this.state.expenseId)}
                />

                <Modal
                    open={this.state.removeWindow}
                    onClose={() => this.toggleModalWindow('removeWindow', 'expenseId')}
                    size='mini'
                    dimmer='inverted'>
                    <Modal.Header>Confirm expense deletion</Modal.Header>
                    <Modal.Content>
                        <p>Are you sure you want to remove this expense?</p>
                        <div style={{textAlign: 'center'}}>
                            <Button negative onClick={this.removeExpense}>Remove</Button>
                            <Button onClick={() => this.toggleModalWindow('removeWindow', 'expenseId')}>Cancel</Button>
                        </div>
                    </Modal.Content>
                </Modal>
            </div>
        );
    }
}

const mapStateToProps = state => ({
    expenses: state.expenses.expenses
});

const mapDispatchToProps = dispatch => ({
    getAllExpenses: () => dispatch(getAllExpenses()),
    addExpense: newExpense => dispatch(addExpense(newExpense)),
    updateExpense: updatedExpense => dispatch(updateExpense(updatedExpense)),
    removeExpense: id => dispatch(removeExpense(id)),
    clearExpenses: () => dispatch(clearExpenses())
});

export default connect(mapStateToProps, mapDispatchToProps)(Expenses);
