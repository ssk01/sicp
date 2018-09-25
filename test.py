def eval(i, success, fail):
(define (analyze-sequence exps)
  (define (sequentially a b)
    (lambda (env succeed fail)
      (a env
         ;; success continuation for calling a
         (lambda (a-value fail2)
           (b env succeed fail2))
         ;; failure continuation for calling a
fail)))

(define (analyze-amb exp)
  (let ((cprocs (map analyze (amb-choices exp))))
    (lambda (env succeed fail)
      (define (try-next choices)
        (if (null? choices)
            (fail)
            ((car choices) env
                           succeed
                           (lambda ()
                             (try-next (cdr choices))))))
(try-next cprocs))))

# # def a():
# #     for i in range(3):
# #         for j in 'abc':
# #             yield (i ,j)
# # b = a()
# # print(b)
# # print(b.__next__())
# # print(b.__next__())
# # print(b.__next__())
# def fck():
#     # pass
#     print('fuck')
# def amb(value, nextValue):
    
# def require(p):
#     if not p:
#         fck()
# def element(items):
#     require(len(items) > 0)
#     amb(items[0], element(items[1:]))


# (define *alternatives* '())

# (define (choose choices)
#   (if (null? choices)
#       (try-again)
#       (call/cc
#        (lambda (cc)
#          (set! *alternatives*
#                (cons
#                 (lambda ()
#                   (cc (choose (cdr choices))))
#                 *alternatives*))
#          (car choices)))))

# (define try-again false)
    
# (call/cc
#  (lambda (cc)
#    (set! try-again
#          (lambda ()
#            (if (null? *alternatives*)
#                (cc '(there are no more values))
#                (let ((next (car *alternatives*)))
#                  (set! *alternatives* (cdr *alternatives*))
#                  (next)))))))