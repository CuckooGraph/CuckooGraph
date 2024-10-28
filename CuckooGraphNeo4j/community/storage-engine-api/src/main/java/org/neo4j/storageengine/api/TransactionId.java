/*
 * Copyright (c) 2002-2020 "Neo4j,"
 * Neo4j Sweden AB [http://neo4j.com]
 *
 * This file is part of Neo4j.
 *
 * Neo4j is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.neo4j.storageengine.api;

/**
 * Transaction id plus meta data that says something about its contents, for comparison.
 */
public class TransactionId
{
    private final long transactionId;
    private final int checksum;
    private final long commitTimestamp;

    public TransactionId( long transactionId, int checksum, long commitTimestamp )
    {
        this.transactionId = transactionId;
        this.checksum = checksum;
        this.commitTimestamp = commitTimestamp;
    }

    /**
     * Transaction id, generated by {@link TransactionIdStore#nextCommittingTransactionId()},
     * here accessible after that transaction being committed.
     */
    public long transactionId()
    {
        return transactionId;
    }

    /**
     * Commit timestamp. Timestamp when transaction with transactionId was committed.
     */
    public long commitTimestamp()
    {
        return commitTimestamp;
    }

    /**
     * Checksum of a transaction, generated from transaction meta data or its contents.
     */
    public int checksum()
    {
        return checksum;
    }

    @Override
    public boolean equals( Object o )
    {
        if ( this == o )
        {
            return true;
        }
        if ( o == null || getClass() != o.getClass() )
        {
            return false;
        }

        TransactionId that = (TransactionId) o;
        return transactionId == that.transactionId &&
               checksum == that.checksum &&
               commitTimestamp == that.commitTimestamp;
    }

    @Override
    public int hashCode()
    {
        int result = (int) (transactionId ^ (transactionId >>> 32));
        result = 31 * result + checksum;
        result = 31 * result + (int) (commitTimestamp ^ (commitTimestamp >>> 32));
        return result;
    }

    @Override
    public String toString()
    {
        return getClass().getSimpleName() + "{" +
                "transactionId=" + transactionId +
                ", checksum=" + checksum +
                ", commitTimestamp=" + commitTimestamp +
                '}';
    }
}
