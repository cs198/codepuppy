angular.module('codepuppy').controller('CommentHolderCtrl',
  function($scope, $modal, $http) {
      $scope.onClicked = function($event) {
        if($scope.commentPermissions) {
          var commentModal = $modal.open({
            templateUrl: '/assets/partials/commentModal/commentModal.html',
            controller: CommentModalCtrl,
            resolve: {
              commentBody: function()  {
                return $scope.commentBody;
              },
            }
          });

          commentModal.result.then(function(comment) {
            $scope.commentBody.comment = comment;

            var createComment = function() {
              var urlParams = {
                submission_file_id: $scope.file.id,
                line_number: $scope.commentBody.line_number,
                num_lines: 1,
                comment: $scope.commentBody.comment,
              };

              // 4 URL params: assignment_id, student_id, date_submited, feedback_released
              $http({
                method: 'POST',
                url: '/submission_files/' + $scope.file.id + '/file_comments.json',
                data: urlParams
              }).success(function(data, status, headers, config) {
              });  
            };

            var replaceComment = function() {
              var urlParams = {
                id: $scope.commentBody.id,
              };
              $http({
                method: 'DELETE',
                url: '/file_comments/' + $scope.commentBody.id,
                data: urlParams
              }).success(function(data, status, headers, config) {
                if($scope.commentBody.comment !== "") {
                  createComment(); 
                }
              }); 

            };
            replaceComment();
          });

          if ($event.stopPropagation) $event.stopPropagation();
          if ($event.preventDefault) $event.preventDefault();
          $event.cancelBubble = true;
          $event.returnValue = false;
        }
    };
  }
);
