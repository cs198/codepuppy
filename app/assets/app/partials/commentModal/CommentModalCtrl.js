var CommentModalCtrl = function($scope, $modalInstance, commentBody) {
  $scope.commentBody = commentBody;
  $scope.initialValue = commentBody.comment;

  $scope.ok = function() {
    $modalInstance.close($scope.commentBody.comment.trim());
  };

  $scope.cancel = function() {
    $scope.commentBody.comment = $scope.initialValue;
    $modalInstance.dismiss('cancel');
  };

  $scope.delete = function() {
    $modalInstance.close("");
  };
};
