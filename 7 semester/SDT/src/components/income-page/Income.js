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
    getAllIncomes,
    addIncome,
    updateIncome,
    removeIncome,
    clearIncomes
} from 'actions/income';

import 'react-table/react-table.css';
import 'styles/Income.css';

// TODO: Add currency convertion support, add processing of planned incomes

const user = localStorage['stUser'] ? JSON.parse(localStorage['stUser']) : {}; // FIX DAT

class Income extends Component {
    static path = '/income';

    constructor(props) {
        super(props);

        this.state = {
            addWindow: false,
            editWindow: false,
            removeWindow: false,
            incomeId: null
        };

        this.incomeRef = firebaseApp.database().ref('income/' + user.uid);
        this.accountsRef = firebaseApp.database().ref('accounts/' + user.uid);

        this.toggleModalWindow = toggleModalWindow.bind(this);
        this.addIncome = this.addIncome.bind(this);
        this.editIncome = this.editIncome.bind(this);
        this.removeIncome = this.removeIncome.bind(this);
    }

    componentWillMount() {
        let newItems = false;
        this.incomeRef.on('child_added', snapshot => {
            if (!newItems) return;

            const newIncome = {
                id: snapshot.key,
                ...snapshot.val()
            };

            this.accountsRef.child(newIncome.account).once('value', s => {
                const account = s.val();
                account.money = +account.money + 
                    convertCurrency(newIncome.currency, account.currency, +newIncome.money);
                this.accountsRef.child(newIncome.account).update(account);
                this.props.addIncome(newIncome);
            });
        });
        this.incomeRef.on('child_changed', snapshot => {
            const updatedIncome = {
                id: snapshot.key,
                ...snapshot.val()
            };
            const oldIncome = this.props.incomes.filter(item => item.id === updatedIncome.id)[0];
            if (!oldIncome) return;

            this.accountsRef.child(oldIncome.account).once('value', s => {
                const account = s.val();
                if (account) {
                    account.money = +account.money -
                        convertCurrency(oldIncome.currency, account.currency, +oldIncome.money) +
                        convertCurrency(updatedIncome.currency, account.currency, +updatedIncome.money);
                    this.accountsRef.child(oldIncome.account).update(account);
                    this.props.updateIncome(updatedIncome);
                }
            });
        });
        this.incomeRef.on('child_removed', snapshot => {
            const removedIncome = snapshot.val();

            this.accountsRef.child(removedIncome.account).once('value', s => {
                const account = s.val();
                if (account) {
                    account.money = +account.money -
                        convertCurrency(removedIncome.currency, account.currency, +removedIncome.money);
                    this.accountsRef.child(removedIncome.account).update(account);
                }
                this.props.removeIncome(snapshot.key);
            });
        });
        this.props.getAllIncomes()
            .then(() => { newItems = true });
    }

    componentWillUnmount() {
        this.incomeRef.off();
        this.props.clearIncomes();
    }

    addIncome(newIncome) {
        this.incomeRef.push(newIncome);
        this.toggleModalWindow('addWindow', 'incomeId');
    }

    editIncome(income) {
        const id = this.state.incomeId;
        this.incomeRef.child(id).update(income);
        this.toggleModalWindow('editWindow', 'incomeId');
    }

    removeIncome() {
        const id = this.state.incomeId;
        this.incomeRef.child(id).remove();
        this.toggleModalWindow('removeWindow', 'incomeId');
    }

    render() {
        const incomes = this.props.incomes.map(item => {
            item.displayMoney = `${item.money} ${item.currency}`;
            item.actions = (
                <ActionsButtons 
                    onEditClick={() => this.toggleModalWindow('editWindow', 'incomeId', item.id)}
                    onRemoveClick={() => this.toggleModalWindow('removeWindow', 'incomeId', item.id)} />
            );
            return item;
        });
        const getIncome = id => {
            return incomes.filter(item => item.id === id)[0];
        };

        return (
            <div className='Income'>
                <Header as='h1' textAlign='center'>Income</Header>
                <Button
                    positive
                    className='add-record-button'
                    onClick={() => this.toggleModalWindow('addWindow', 'incomeId')}>Add New Record</Button>
                <ReactTable
                    data={incomes}
                    columns={tableHeaders.INCOME}
                    defaultPageSize={10}
                    className='-striped -highlight'
                />

                <RecordWindow 
                    isOpen={this.state.addWindow}
                    headerText='Add new record'
                    submitText='Add'
                    onSubmit={this.addIncome}
                    onCancel={() => this.toggleModalWindow('addWindow', 'incomeId')}
                    recordType='income'
                    allowFutureDate={false}
                />

                <RecordWindow 
                    isOpen={this.state.editWindow}
                    headerText='Edit income'
                    submitText='Edit'
                    onSubmit={this.editIncome}
                    onCancel={() => this.toggleModalWindow('editWindow', 'incomeId')}
                    recordType='income'
                    allowFutureDate={false}
                    values={getIncome(this.state.incomeId)}
                />

                <Modal
                    open={this.state.removeWindow}
                    onClose={() => this.toggleModalWindow('removeWindow', 'incomeId')}
                    size='mini'
                    dimmer='inverted'>
                    <Modal.Header>Confirm income deletion</Modal.Header>
                    <Modal.Content>
                        <p>Are you sure you want to remove this income?</p>
                        <div style={{textAlign: 'center'}}>
                            <Button negative onClick={this.removeIncome}>Remove</Button>
                            <Button onClick={() => this.toggleModalWindow('removeWindow', 'incomeId')}>Cancel</Button>
                        </div>
                    </Modal.Content>
                </Modal>
            </div>
        );
    }
}

const mapStateToProps = state => ({
    incomes: state.income.incomes
});

const mapDispatchToProps = dispatch => ({
    getAllIncomes: () => dispatch(getAllIncomes()),
    addIncome: newIncome => dispatch(addIncome(newIncome)),
    updateIncome: updatedIncome => dispatch(updateIncome(updatedIncome)),
    removeIncome: id => dispatch(removeIncome(id)),
    clearIncomes: () => dispatch(clearIncomes())
});

export default connect(mapStateToProps, mapDispatchToProps)(Income);
