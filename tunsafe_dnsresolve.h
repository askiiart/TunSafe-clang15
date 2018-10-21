#ifndef TUNSAFE_DNSRESOLVE_H_
#define TUNSAFE_DNSRESOLVE_H_

#include "tunsafe_threading.h"
#include "tunsafe_ipaddr.h"

class DnsBlocker;

class DnsResolverCanceller {
public:
  DnsResolverCanceller() : cancel_(false) {}
  void Cancel();
  void Reset() { cancel_ = false; }
  bool is_cancelled() { return cancel_; }
public:
  bool cancel_;
  ConditionVariable condvar_;
};

class DnsResolver {
public:
  explicit DnsResolver(DnsBlocker *dns_blocker);
  ~DnsResolver();

  bool Resolve(const char *hostname, IpAddr *result);
  void ClearCache();

  void Cancel() { token_.Cancel(); }
  void ResetCancel() { token_.Reset(); }
private:
  struct Entry {
    std::string name;
    IpAddr ip;
    Entry(const std::string &name, const IpAddr &ip) : name(name), ip(ip) {}
  };
  std::vector<Entry> cache_;
  DnsBlocker *dns_blocker_;
  DnsResolverCanceller token_;
};

#endif  // TUNSAFE_DNSRESOLVE_H_