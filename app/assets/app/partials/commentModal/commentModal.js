var CommentModalCtrl = function($scope, $modalInstance, commentBody) {

    $scope.commentBody = commentBody;
    $scope.initialValue = commentBody.comment;

    //TODO: Set initial value to value of comment clicked on.
    // $scope.commentBody.comment = "Initial value";
    
    $scope.ok = function() {
       $modalInstance.close($scope.commentBody.comment);
    };

    $scope.cancel = function() {
		$scope.commentBody.comment = $scope.initialValue;
		$modalInstance.dismiss('cancel');
    };

};