#!/usr/bin/env bash

# Bash strict mode: Fail if any command fails, undefined variables exist, or piped commands fail
set -euo pipefail

readonly BACKUP_DIR="/var/log/backup"
readonly MAX_RETRIES=3

log_error() {
    echo "[$(date +'%Y-%m-%dT%H:%M:%S')] ERROR: $*" >&2
}

verify_environment() {
    if [[ $EUID -ne 0 ]]; then
        log_error "This script must run with root permissions."
        exit 1
    fi

    if [[ ! -d "$BACKUP_DIR" ]]; then
        echo "Target location missing. Allocating path: $BACKUP_DIR"
        mkdir -p "$BACKUP_DIR"
    fi
}

execute_rotation() {
    local source_target="${1}"
    
    if [[ ! -f "$source_target" ]]; then
        log_error "Target file ${source_target} cannot be verified."
        return 2
    fi

    echo "Compressing file target: ${source_target}"
    tar -czf "${BACKUP_DIR}/log_archive_$(date +%F).tar.gz" "${source_target}"
}

main() {
    if [[ $# -lt 1 ]]; then
        log_error "Usage: $0 <absolute_path_to_log>"
        exit 64
    fi

    verify_environment
    execute_rotation "$1"
}

# Pass all incoming execution arguments safely to runtime main
main "$@"
